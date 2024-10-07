package com.project.sparrow.ui.tools;

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.project.sparrow.R;
import com.project.sparrow.databinding.FragmentToolsBinding;
import com.project.sparrow.tools.KeyboardActivity;
import com.project.sparrow.tools.MouseActivity;

public class ToolsFragment extends Fragment {

    private FragmentToolsBinding binding;
    private Button btn_keyboard, btn_mouse;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        ToolsViewModel toolsViewModel =
                new ViewModelProvider(this).get(ToolsViewModel.class);

        binding = FragmentToolsBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

//        final TextView textView = binding.textGallery;
//        galleryViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);

        return root;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        btn_keyboard = getActivity().findViewById(R.id.tools_keyboard);
        btn_mouse = getActivity().findViewById(R.id.tools_mouse);

        btn_keyboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(getActivity(), KeyboardActivity.class);
                startActivity(intent);
            }
        });

        btn_mouse.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(getActivity(), MouseActivity.class);
                startActivity(intent);
            }
        });
    }



    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}